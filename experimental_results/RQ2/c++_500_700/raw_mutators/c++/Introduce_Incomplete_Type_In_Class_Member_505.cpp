//header file
#pragma once
#include "Mutator_base.h"

/**
 * introduce_incomplete_type_in_class_member_505
 */ 
class MutatorFrontendAction_505 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(505)

private:
    class MutatorASTConsumer_505 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_505(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Incomplete_Type_In_Class_Member_505.h"

// ========================================================================================================
#define MUT505_OUTPUT 1

void MutatorFrontendAction_505::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      
      //Get the source code text of target node
      auto className = CRD->getNameAsString();
      if (className.empty())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "/*mut505*/" + className + " " + "member;\n";
      SourceLocation insertLoc = CRD->getBraceRange().getBegin().getLocWithOffset(1);
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, insertion);
    }
}
  
void MutatorFrontendAction_505::MutatorASTConsumer_505::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}