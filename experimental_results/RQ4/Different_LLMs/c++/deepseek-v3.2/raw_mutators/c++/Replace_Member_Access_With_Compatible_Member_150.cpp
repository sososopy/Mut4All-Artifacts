//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Access_With_Compatible_Member_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(150)

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Access_With_Compatible_Member_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getLocation()))
        return;
      //Get the source code text of target node
      auto *FD = dyn_cast<FieldDecl>(ME->getMemberDecl());
      if (!FD) return;
      if (!FD->getType()->isIntegralOrEnumerationType()) return;
      auto *RD = dyn_cast<CXXRecordDecl>(FD->getParent());
      if (!RD || !RD->isCompleteDefinition()) return;
      std::vector<FieldDecl*> candidates;
      for (auto *D : RD->decls()) {
        if (auto *CandidateFD = dyn_cast<FieldDecl>(D)) {
          if (CandidateFD == FD) continue;
          if (!CandidateFD->getType()->isIntegralOrEnumerationType()) continue;
          if (CandidateFD->getAccess() != AS_public && CandidateFD->getAccess() != AS_none) continue;
          if (ME->isArrow()) {
            if (CandidateFD->getType()->isIntegralOrEnumerationType() && FD->getType()->isIntegralOrEnumerationType()) {
              candidates.push_back(CandidateFD);
            }
          } else {
            if (CandidateFD->getType()->isIntegralOrEnumerationType() && FD->getType()->isIntegralOrEnumerationType()) {
              candidates.push_back(CandidateFD);
            }
          }
        }
      }
      if (candidates.empty()) return;
      size_t idx = getrandom::getRandomIndex(candidates.size() - 1);
      auto *NewFD = candidates[idx];
      //Perform mutation on the source code text by applying string replacement
      std::string NewName = NewFD->getNameAsString();
      SourceRange NameRange = ME->getMemberNameInfo().getSourceRange();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(NameRange, NewName);
    }
}
  
void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = memberExpr(hasDeclaration(fieldDecl(hasType(isInteger())).bind("FieldDecl"))).bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}