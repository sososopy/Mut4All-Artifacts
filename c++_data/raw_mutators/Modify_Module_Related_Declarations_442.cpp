//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Module_Related_Declarations_442
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
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/modify_module_related_declarations_442.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (!VD->isFileVarDecl() || VD->hasExternalStorage())
        return;

      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation StartLoc = VD->getBeginLoc();
      std::string VarDeclText = Rewrite.getRewrittenText(VD->getSourceRange());

      if (VarDeclText.find("export") == std::string::npos) {
        std::string MutatedText = "export " + VarDeclText;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(isExpansionInMainFile(), isGlobalVarDecl()).bind("GlobalVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}