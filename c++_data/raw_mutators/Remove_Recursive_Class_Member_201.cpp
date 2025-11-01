//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Recursive_Class_Member_201
 */ 
class MutatorFrontendAction_201 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(201)

private:
    class MutatorASTConsumer_201 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_201(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_201.h"

// ========================================================================================================
#define MUT201_OUTPUT 1

void MutatorFrontendAction_201::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recursiveMember")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
        return;

      for (auto *Field : RD->fields()) {
        if (Field->getType()->isRecordType() && Field->getType()->getAsCXXRecordDecl() == RD) {
          SourceRange range = Field->getSourceRange();
          std::string comment = "// " + Lexer::getSourceText(CharSourceRange::getTokenRange(range), *Result.SourceManager, Result.Context->getLangOpts()).str() + " // Removed recursive member";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), comment);
        }
      }
    }
}
  
void MutatorFrontendAction_201::MutatorASTConsumer_201::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(fieldDecl(hasType(recordType(hasDeclaration(equalsBoundNode("recursiveMember"))))))).bind("recursiveMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}