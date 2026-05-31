//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_Parameters_458
 */ 
class MutatorFrontendAction_458 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(458)

private:
    class MutatorASTConsumer_458 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_458(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Instantiation_Parameters_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(TemplateInstantiation)) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TI->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template instantiation parameters
      std::string newDeclaration = declaration;
      size_t pos = newDeclaration.find('<');
      if (pos != std::string::npos) {
        size_t endPos = newDeclaration.find('>');
        if (endPos != std::string::npos) {
          std::string params = newDeclaration.substr(pos + 1, endPos - pos - 1);
          // Modify the template parameters (e.g., change int to float)
          std::string newParams = "float";
          newDeclaration.replace(pos + 1, endPos - pos - 1, newParams);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), newDeclaration);
    }
}
  
void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind(TemplateInstantiation);
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}