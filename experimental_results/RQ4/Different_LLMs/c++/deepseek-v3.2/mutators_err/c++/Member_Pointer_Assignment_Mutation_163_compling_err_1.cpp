//header file
#pragma once
#include "Mutator_base.h"

/**
 * Member_Pointer_Assignment_Mutation_163
 */ 
class MutatorFrontendAction_163 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(163)

private:
    class MutatorASTConsumer_163 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_163(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> allClasses;
        std::vector<const clang::FieldDecl *> allDataMembers;
        std::vector<const clang::CXXMethodDecl *> allMemberFunctions;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AS = Result.Nodes.getNodeAs<clang::BinaryOperator>("AssignStmt")) {
      //Filter nodes in header files
      if (!AS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AS->getLocation()))
        return;
      if (!AS->isAssignmentOp())
        return;
      //Get the source code text of target node
      auto *LHS = AS->getLHS()->IgnoreParenImpCasts();
      auto *RHS = AS->getRHS()->IgnoreParenImpCasts();
      auto *LHSType = LHS->getType().getTypePtrOrNull();
      if (!LHSType)
        return;
      //Check if LHS is a pointer to member
      bool isDataMemberPtr = LHSType->isMemberDataPointerType();
      bool isMemberFunctionPtr = LHSType->isMemberFunctionPointerType();
      if (!isDataMemberPtr && !isMemberFunctionPtr)
        return;
      //Collect candidate members for mutation
      if (allClasses.empty() || (allDataMembers.empty() && allMemberFunctions.empty()))
        return;
      //Select a random class different from the expected class
      const clang::CXXRecordDecl *targetClass = nullptr;
      if (isDataMemberPtr) {
        auto *MPT = LHSType->getAs<clang::MemberPointerType>();
        if (!MPT)
          return;
        auto *ClassType = MPT->getClass()->getAsCXXRecordDecl();
        if (!ClassType)
          return;
        //Filter out classes that are the same as the expected class
        std::vector<const clang::CXXRecordDecl *> otherClasses;
        for (auto *C : allClasses) {
          if (C != ClassType)
            otherClasses.push_back(C);
        }
        if (otherClasses.empty())
          return;
        size_t classIdx = getrandom::getRandomIndex(otherClasses.size() - 1);
        targetClass = otherClasses[classIdx];
        //Select a random data member from the target class
        std::vector<const clang::FieldDecl *> candidates;
        for (auto *FD : allDataMembers) {
          if (FD->getParent() == targetClass)
            candidates.push_back(FD);
        }
        if (candidates.empty())
          return;
        size_t memberIdx = getrandom::getRandomIndex(candidates.size() - 1);
        auto *selectedMember = candidates[memberIdx];
        //Perform mutation on the source code text by applying string replacement
        std::string newRHS = "&" + selectedMember->getNameAsString();
        SourceRange RHSRange = RHS->getSourceRange();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RHSRange), newRHS);
      } else if (isMemberFunctionPtr) {
        auto *MPT = LHSType->getAs<clang::MemberPointerType>();
        if (!MPT)
          return;
        auto *ClassType = MPT->getClass()->getAsCXXRecordDecl();
        if (!ClassType)
          return;
        //Filter out classes that are the same as the expected class
        std::vector<const clang::CXXRecordDecl *> otherClasses;
        for (auto *C : allClasses) {
          if (C != ClassType)
            otherClasses.push_back(C);
        }
        if (otherClasses.empty())
          return;
        size_t classIdx = getrandom::getRandomIndex(otherClasses.size() - 1);
        targetClass = otherClasses[classIdx];
        //Select a random member function from the target class
        std::vector<const clang::CXXMethodDecl *> candidates;
        for (auto *MD : allMemberFunctions) {
          if (MD->getParent() == targetClass && !MD->isStatic())
            candidates.push_back(MD);
        }
        if (candidates.empty())
          return;
        size_t memberIdx = getrandom::getRandomIndex(candidates.size() - 1);
        auto *selectedMember = candidates[memberIdx];
        //Perform mutation on the source code text by applying string replacement
        std::string newRHS = "&" + selectedMember->getNameAsString();
        SourceRange RHSRange = RHS->getSourceRange();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RHSRange), newRHS);
      }
    }
    //Collect class and member information
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      allClasses.push_back(RD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      allDataMembers.push_back(FD);
    }
    else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodDecl")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      allMemberFunctions.push_back(MD);
    }
}
  
void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto assignMatcher = binaryOperator(isAssignmentOperator()).bind("AssignStmt");
    auto classMatcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    auto fieldMatcher = fieldDecl().bind("FieldDecl");
    auto methodMatcher = cxxMethodDecl().bind("MethodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(assignMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}