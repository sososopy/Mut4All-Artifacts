//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Atomic_Initialization_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(461)

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::VarDecl *> atomic_vars;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/insert_atomic_initialization_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("AtomicVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isAtomicType() == false)
        return;
      if (DL->hasInit() == false)
        return;
      atomic_vars.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Vars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->isAtomicType() == false)
        return;
      if (DL->hasInit() == false)
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
      auto init_text = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getInit()->getSourceRange());
      llvm::outs() << init_text << '\n';
      if (init_text.find('(') == string::npos)
        text = stringutils::replace_str(text, init_text, init_text);
      else {
        auto type_text = stringutils::rangetoStr(
            *(Result.SourceManager),
            DL->getType()->getAs<clang::AtomicType>()
                ->getValueType()
                ->getTypeLoc()
                .getSourceRange());
        text = stringutils::replace_str(
            text, init_text,
            "(" + type_text + ")" + init_text.substr(init_text.find('(')));
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          text);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("NewVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          DL->getSourceRange());
      llvm::outs() << text << '\n';
      if (DL->getType()->isAtomicType() == false) {
        auto type_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 DL->getTypeSourceInfo()
                                                     ->getTypeLoc()
                                                     .getSourceRange());
        text = "_Atomic(" + type_text + ")" + text.substr(type_text.size());
      }
      if (DL->hasInit() == false) {
        text += "=0";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          text);
    }
}

void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasType(isAtomic())).bind("Vars");
    auto matcher2 = varDecl().bind("NewVars");
    auto matcher3 = varDecl().bind("AtomicVars");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
    if (callback.atomic_vars.empty()) {
      MatchFinder finder;
      finder.addMatcher(matcher2, &callback);
      finder.matchAST(Context);
    } else {
      MatchFinder finder;
      finder.addMatcher(matcher, &callback);
      finder.matchAST(Context);
    }
}