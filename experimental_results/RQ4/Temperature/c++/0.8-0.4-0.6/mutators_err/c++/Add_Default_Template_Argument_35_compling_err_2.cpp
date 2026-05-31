//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_default_template_argument_35
 */ 
class MutatorFrontendAction_35 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(35)

private:
    class MutatorASTConsumer_35 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_35(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_base.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        if (FTD->getTemplateParameters()->size() > 0) {
            auto *TP = FTD->getTemplateParameters()->getParam(0);
            if (auto *TTP = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TP)) {
                if (!TTP->hasDefaultArgument()) {
                    SourceLocation insertLoc = TTP->getEndLoc().getLocWithOffset(1);
                    std::string defaultArg = " = int";
                    Rewrite.InsertTextAfter(insertLoc, defaultArg);
                }
            }
        }
    }
}

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}