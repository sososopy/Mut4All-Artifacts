//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Default_Member_Initializer_With_Lambda_345
 */ 
class MutatorFrontendAction_345 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(345)

private:
    class MutatorASTConsumer_345 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_345(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> candidateStructs;
    };
};

//source file
#include "../include/Insert_Default_Member_Initializer_With_Lambda_345.h"

// ========================================================================================================
#define MUT345_OUTPUT 1

void MutatorFrontendAction_345::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isStruct() || RD->isLambda())
        return;
      if (!RD->isCompleteDefinition())
        return;
      //Record candidate structs
      candidateStructs.push_back(RD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      const clang::CXXRecordDecl* RD = FD->getParent();
      if (!RD || !RD->isStruct())
        return;
      //Check if this field already has a default member initializer
      if (FD->hasInClassInitializer())
        return;
      //Find a suitable captured member
      std::vector<const clang::FieldDecl*> candidateCapturedMembers;
      for (auto field : RD->fields()) {
        if (field != FD && !field->hasInClassInitializer()) {
          candidateCapturedMembers.push_back(field);
        }
      }
      //If no suitable captured member exists, we will introduce a dummy member
      bool needDummy = candidateCapturedMembers.empty();
      //Perform mutation on the source code text by applying string replacement
      std::string mutation;
      if (needDummy) {
        //Insert dummy member before the selected field
        mutation = "int dummy = 0;\n";
        Rewrite.InsertTextBefore(FD->getBeginLoc(), mutation);
        //Now create the lambda initializer using dummy
        mutation = FD->getNameAsString() + " = []() { return [=]() { return dummy; }(); }();";
      } else {
        //Select a random captured member
        size_t idx = getrandom::getRandomIndex(candidateCapturedMembers.size() - 1);
        const clang::FieldDecl* captured = candidateCapturedMembers[idx];
        mutation = FD->getNameAsString() + " = []() { return [=]() { return " + captured->getNameAsString() + "; }(); }();";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FD->getSourceRange(), mutation);
    }
}
  
void MutatorFrontendAction_345::MutatorASTConsumer_345::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.matchAST(Context);
}