//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrain_NonType_Template_Parameter_Pack_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(135)

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Constrain_NonType_Template_Parameter_Pack_135.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto &SM = Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto &TheRewriter = Rewrite;
      
      auto TemplateParams = FD->getTemplateParameters();
      if (TemplateParams->size() < 1)
        return;

      // Create a new concept definition
      std::string conceptDef = "\n/*mut135*/template<typename T> concept ConceptConstraint = true;\n";
      TheRewriter.InsertTextBefore(FD->getBeginLoc(), conceptDef);

      // Apply requires clause to the function template
      std::string requiresClause = "requires (ConceptConstraint<decltype(values)> && ...)";
      auto FuncDecl = FD->getTemplatedDecl();
      auto FuncLoc = FuncDecl->getBeginLoc();
      TheRewriter.InsertTextAfterToken(FuncLoc, requiresClause);
    }
}

void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}