//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_480
 */ 
class MutatorFrontendAction_480 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(480)

private:
    class MutatorASTConsumer_480 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_480(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FD->getBody()->getSourceRange());

            std::string templateCode = R"(
template<int N>
struct RecursiveTemplate {
    static constexpr int value = RecursiveTemplate<N - 1>::value;
};

template<>
struct RecursiveTemplate<0> {
    static constexpr int value = 0;
};

int triggerRecursion() {
    return RecursiveTemplate<1000>::value;
}
)";

            functionBody.insert(functionBody.find("{") + 1, "\n/*mut480*/" + templateCode + "\n");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    }
}

void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}