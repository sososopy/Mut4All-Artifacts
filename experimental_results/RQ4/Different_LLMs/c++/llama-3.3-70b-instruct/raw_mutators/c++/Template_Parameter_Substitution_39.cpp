```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Substitution_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Template_Parameter_Substitution_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template parameter with different types or values
      std::string mutatedDeclaration = declaration;
      size_t templateParamPos = declaration.find('<');
      if (templateParamPos != std::string::npos) {
        size_t templateParamEndPos = declaration.find('>');
        if (templateParamEndPos != std::string::npos) {
          std::string templateParam = declaration.substr(templateParamPos + 1, templateParamEndPos - templateParamPos - 1);
          // Replace template parameter with different types or values
          if (templateParam == "int") {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "float");
          } else if (templateParam == "float") {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "char");
          } else {
            mutatedDeclaration.replace(templateParamPos + 1, templateParamEndPos - templateParamPos - 1, "int");
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    CallExprMatcher matcher = callExpr(hasTemplateArgument(0, anyValue())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}