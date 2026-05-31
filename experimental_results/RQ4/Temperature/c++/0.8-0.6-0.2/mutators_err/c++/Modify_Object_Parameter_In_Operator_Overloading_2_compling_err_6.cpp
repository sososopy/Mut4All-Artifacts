//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_object_parameter_in_operator_overloading_2
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)

private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_object_parameter_in_operator_overloading_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorOverload")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->isOverloadedOperator() && MT->getNumParams() > 0) {
          const ParmVarDecl* firstParam = MT->getParamDecl(0);
          if (firstParam->getType()->isPointerType()) {
              //Get the source code text of target node
              auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                         MT->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              std::string newDeclaration = "/*mut2*/" + declaration;
              size_t pos = newDeclaration.find("this");
              if (pos != std::string::npos) {
                  newDeclaration.replace(pos, 4, MT->getParent()->getNameAsString() + " &");
              }
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
          }
      }
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator(), hasParameter(0, parmVarDecl(hasType(pointsTo(anything()))))).bind("OperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}