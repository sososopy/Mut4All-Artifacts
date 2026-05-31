//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_445
 */ 
class MutatorFrontendAction_445 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(445)

private:
    class MutatorASTConsumer_445 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_445(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody() && FD->isTemplateInstantiation()) {
            auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            std::string recursiveTemplate = "template<int N>\nstruct RecursiveTemplate {\n"
                                            "    static const int value = N + RecursiveTemplate<N-1>::value;\n"
                                            "};\n"
                                            "template<>\nstruct RecursiveTemplate<0> {\n"
                                            "    static const int value = 0;\n"
                                            "};\n";
            funcDecl.insert(0, "/*mut445*/\n" + recursiveTemplate);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
        }
    }
}

void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}