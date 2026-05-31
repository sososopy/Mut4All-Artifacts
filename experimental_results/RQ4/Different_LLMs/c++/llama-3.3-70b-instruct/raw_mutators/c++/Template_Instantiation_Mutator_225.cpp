//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_Mutator_225
 */ 
class MutatorFrontendAction_225 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(225)

private:
    class MutatorASTConsumer_225 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_225(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Instantiation_Mutator_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template arguments with different types
      std::string mutatedDeclaration = declaration;
      size_t templateParamPos = declaration.find('<');
      size_t templateParamEndPos = declaration.find('>');
      if (templateParamPos != std::string::npos && templateParamEndPos != std::string::npos) {
        std::string templateParams = declaration.substr(templateParamPos + 1, templateParamEndPos - templateParamPos - 1);
        std::vector<std::string> params;
        size_t commaPos = templateParams.find(',');
        while (commaPos != std::string::npos) {
          params.push_back(templateParams.substr(0, commaPos));
          templateParams = templateParams.substr(commaPos + 1);
          commaPos = templateParams.find(',');
        }
        params.push_back(templateParams);
        // Replace each template parameter with a different type
        for (size_t i = 0; i < params.size(); ++i) {
          if (params[i] == "int") {
            params[i] = "float";
          } else if (params[i] == "float") {
            params[i] = "double";
          } else if (params[i] == "double") {
            params[i] = "int";
          }
        }
        std::string mutatedTemplateParams;
        for (size_t i = 0; i < params.size(); ++i) {
          mutatedTemplateParams += params[i];
          if (i < params.size() - 1) {
            mutatedTemplateParams += ",";
          }
        }
        mutatedDeclaration = declaration.substr(0, templateParamPos + 1) + mutatedTemplateParams + declaration.substr(templateParamEndPos);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}