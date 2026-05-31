//header file
#pragma once
#include "Mutator_base.h"

/**
 * Explicit_Object_Parameter_Template_Instantiation_Duplication_405
 */ 
class MutatorFrontendAction_405 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(405)

private:
    class MutatorASTConsumer_405 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_405(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Explicit_Object_Parameter_Template_Instantiation_Duplication_405.h"

// ========================================================================================================
#define MUT405_OUTPUT 1

void MutatorFrontendAction_405::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if method has explicit object parameter (C++23)
      if (!MT->isExplicitObjectMemberFunction())
        return;
      //Check if method is in a template class/struct
      const CXXRecordDecl* recordDecl = MT->getParent();
      if (!recordDecl || !recordDecl->isTemplateDecl())
        return;
      //Check if method has a body
      if (!MT->hasBody())
        return;
      Stmt* body = MT->getBody();
      //Find call expressions within the body that involve the explicit object parameter
      std::vector<CallExpr*> targetCalls;
      for (Stmt* child : body->children()) {
        if (auto* callExpr = dyn_cast<CallExpr>(child)) {
          //Check if call involves the explicit object parameter or member of same class
          Expr* callee = callExpr->getCallee();
          if (callee) {
            //Check if callee references the explicit object parameter or a member of the same template class
            //Simplified check: if callee is a member expression referencing the same class
            if (auto* memberExpr = dyn_cast<MemberExpr>(callee)) {
              QualType baseType = memberExpr->getBase()->getType();
              if (baseType.getAsString().find(recordDecl->getNameAsString()) != std::string::npos) {
                targetCalls.push_back(callExpr);
              }
            }
          }
        }
      }
      //If no target calls found, return
      if (targetCalls.empty())
        return;
      //Select a random call to duplicate
      int index = getrandom::getRandomIndex(targetCalls.size() - 1);
      CallExpr* selectedCall = targetCalls[index];
      //Get the source code text of target node
      std::string callText = stringutils::rangetoStr(*(Result.SourceManager), selectedCall->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Insert duplicate call after the original call
      SourceLocation insertLoc = selectedCall->getEndLoc();
      std::string duplicateCall = "\n" + callText;
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, duplicateCall);
    }
}
  
void MutatorFrontendAction_405::MutatorASTConsumer_405::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("MethodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}