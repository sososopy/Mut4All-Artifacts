//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_TemplateArgumentType_124
 */ 
class MutatorFrontendAction_124 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(124)

private:
    class MutatorASTConsumer_124 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_124(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Change_TemplateArgumentType_124.h"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = FT->getTemplateParameters();
      for (auto I = templateParams->begin(); I!= templateParams->end(); ++I) {
        if (auto nonTypeParam = dyn_cast<clang::NonTypeTemplateParmDecl>(*I)) {
          if (nonTypeParam->getType()->isIntegerType()) {
            //Perform mutation on the source code text by applying string replacement
            std::string originalType = nonTypeParam->getType()->getAsString();
            std::string newType = "unsigned int";
            llvm::StringRef declaration = clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(FT->getSourceRange()), 
                *Result.SourceManager, 
                clang::LangOptions()
            );
            std::string declarationStr = declaration.str();
            size_t pos = declarationStr.find(originalType);
            if (pos!= std::string::npos) {
              declarationStr.replace(pos, originalType.length(), newType);
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(FT->getSourceRange()), declarationStr);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}