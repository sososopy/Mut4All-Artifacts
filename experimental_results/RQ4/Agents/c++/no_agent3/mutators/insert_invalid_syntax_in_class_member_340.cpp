//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Syntax_In_Class_Member_340
 */ 
class MutatorFrontendAction_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(340)

private:
    class MutatorASTConsumer_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_340(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_invalid_syntax_in_class_member_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager), CRD->getSourceRange());
      std::string invalidSyntax = "\n    x // Invalid syntax introduced\n";
      if (content.find("public:") != std::string::npos) {
        content.insert(content.find("public:") + 7, invalidSyntax);
      } else if (content.find("private:") != std::string::npos) {
        content.insert(content.find("private:") + 8, invalidSyntax);
      } else if (content.find("protected:") != std::string::npos) {
        content.insert(content.find("protected:") + 10, invalidSyntax);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CRD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}