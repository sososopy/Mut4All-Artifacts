//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Name_Specifier_Pointer_To_Member_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberFunction")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (const auto *Parent = dyn_cast<CXXRecordDecl>(MT->getParent())) {
        if (Parent->isCompleteDefinition()) {
          std::string className = Parent->getNameAsString();
          std::string methodName = MT->getNameAsString();
          std::string pointerToMember = "void (" + className + "::*)()";
          std::string nestedSpecifier = className + "::" + methodName;

          std::string mutationText = "/*mut155*/" + nestedSpecifier + " = &" + className + "::" + methodName + ";";
          SourceLocation insertLoc = MT->getBeginLoc();
          Rewrite.InsertTextBefore(insertLoc, mutationText);
        }
      }
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("MemberFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}