//header file
#pragma once
#include "Mutator_base.h"

/**
 * Module_Codegen_Mismatch_442
 */ 
class MutatorFrontendAction_442 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(442)

private:
    class MutatorASTConsumer_442 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_442(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ImportDecl *> import_decls;
    };
};

//source file
#include "../include/Module_Codegen_Mismatch_442.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ID = Result.Nodes.getNodeAs<clang::ImportDecl>("ImportDecl")) {
    if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ID->getBeginLoc()))
      return;
    import_decls.push_back(ID);
  } else if (auto *ID =
                 Result.Nodes.getNodeAs<clang::InclusionDirective>("Include")) {
    if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ID->getBeginLoc()))
      return;
    if (ID->wasNotResolved()) {
      auto ins = "import " + ID->getFileName().str() + ';';
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(ID->getBeginLoc(), ID->getEndLoc(), ins);
    }
  } else if (auto *ID = Result.Nodes.getNodeAs<clang::ImportDecl>("Import")) {
    if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                   ID->getBeginLoc()))
      return;
    if (ID->getImportedModule() == nullptr) {
      auto ins = "#include \"" + ID->getImportedModule()->getFullModuleName().str() + ".h\"";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(ID->getBeginLoc(), ID->getEndLoc(), ins);
    }
  }
}

void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto import_matcher = importDecl().bind("Import");
  matchFinder.addMatcher(import_matcher, new Callback(TheRewriter));
  matchFinder.matchAST(Context);
  MatchFinder finder;
  auto include_matcher = inclusionDirective().bind("Include");
  finder.addMatcher(include_matcher, new Callback(TheRewriter));
  finder.matchAST(Context);
}