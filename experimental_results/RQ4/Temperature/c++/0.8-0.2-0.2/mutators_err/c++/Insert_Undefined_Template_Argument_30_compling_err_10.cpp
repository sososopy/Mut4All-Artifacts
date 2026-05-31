//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_undefined_template_argument_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_undefined_template_argument_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TSD = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Result.SourceManager->getExpansionLoc(TSD->getTemplateNameLoc())))
        return;
      //Get the source code text of target node
      auto sourceRange = TSD->getTemplateNameLoc();
      std::string originalCode = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                                      *Result.SourceManager, 
                                                      Result.Context->getLangOpts()).str();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = originalCode;
      std::size_t angleBracketPos = mutatedCode.find('<') + 1;
      if(angleBracketPos != std::string::npos) {
          mutatedCode.insert(angleBracketPos, " UndefinedType,");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedCode);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}