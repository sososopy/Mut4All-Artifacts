//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_Type_410
 */ 
class MutatorFrontendAction_410 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(410)

private:
    class MutatorASTConsumer_410 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_410(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::TemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Replace_Template_Argument_Type_410.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = TD->getTemplateParameters();
      for (auto param = templateParams.begin(); param != templateParams.end(); ++param) {
        if ((*param)->isNonTypeTemplateParameter()) {
          //Perform mutation on the source code text by applying string replacement
          std::string newType = "std::uint64_t";
          std::string declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           TD->getSourceRange());
          declaration.replace(declaration.find((*param)->getNameAsString()), (*param)->getNameAsString().length(), newType);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
        }
      }
    }
}

void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}