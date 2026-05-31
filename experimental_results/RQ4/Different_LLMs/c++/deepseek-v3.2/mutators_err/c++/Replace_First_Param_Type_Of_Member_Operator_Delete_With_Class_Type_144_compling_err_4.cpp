//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_First_Param_Type_Of_Member_Operator_Delete_With_Class_Type_144
 */ 
class MutatorFrontendAction_144 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(144)

private:
    class MutatorASTConsumer_144 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_144(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_First_Param_Type_Of_Member_Operator_Delete_With_Class_Type_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MemberOperatorDelete")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      // Ensure it's a member operator delete
      if (!FD->isCXXClassMember() || !FD->isOverloadedOperator() || FD->getOverloadedOperator() != clang::OO_Delete)
         return;
      // Ensure it has at least one parameter
      if (FD->getNumParams() == 0)
        return;
      // Get the enclosing class
      auto *CXXRD = dyn_cast<clang::CXXRecordDecl>(FD->getDeclContext());
      if (!CXXRD)
        return;
      // Get the first parameter
      auto *FirstParam = FD->getParamDecl(0);
      if (!FirstParam)
        return;
      // Get the source range of the first parameter's type
      auto TypeRange = FirstParam->getTypeSourceInfo()->getTypeLoc().getSourceRange();
      // Get the class name
      std::string ClassName = CXXRD->getNameAsString();
      if (ClassName.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(TypeRange, ClassName);
    }
}
  
void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(clang::ast_matchers::isCXXClassMember(), hasOverloadedOperatorName("delete")).bind("MemberOperatorDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}