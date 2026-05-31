//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> processedFunctions;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (processedFunctions.find(FD) != processedFunctions.end())
            return;
        processedFunctions.insert(FD);

        auto funcName = FD->getNameAsString();
        if (funcName.empty())
            return;

        std::string recursiveTemplate = R"(
template<int N>
struct RecursiveTemplate {
    static void call() {
        if constexpr (N > 0) {
            RecursiveTemplate<N - 1>::call();
        }
    }
};

)";

        std::string insertion = recursiveTemplate + "/*mut452*/RecursiveTemplate<10>::call();\n";
        if (FD->hasBody()) {
            auto bodyRange = FD->getBody()->getSourceRange();
            Rewrite.InsertTextAfterToken(bodyRange.getBegin(), insertion);
        }
    }
}

void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}