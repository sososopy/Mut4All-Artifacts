//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Class_Member_Init_Undeclared_Member_335
 */ 
class MutatorFrontendAction_335 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(335)

private:
    class MutatorASTConsumer_335 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_335(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Class_Member_Init_Undeclared_Member_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isExplicit() && !MT->isImplicit() && !MT->isDeleted()) {
        auto initlist = MT->inits();
        for (auto init : initlist) {
          auto member = init->getMember();
          if (member == nullptr)
            continue;
          auto membername = member->getNameAsString();
          auto parent = member->getParent();
          auto parentname = parent->getNameAsString();
          llvm::outs() << "member: " << membername << " parent: " << parentname
                       << '\n';
          if (parent->getNameAsString() !=
              MT->getParent()->getNameAsString()) { // not declared in the class
            auto content = stringutils::rangetoStr(
                *(Result.SourceManager), MT->getSourceRange());
            llvm::outs() << content << '\n';
            if (content.find(':') != string::npos) {
              content.insert(content.find(':') + 1, membername + "(0), ");
            }
            llvm::outs() << content << '\n';
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(MT->getSourceRange()), content);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}