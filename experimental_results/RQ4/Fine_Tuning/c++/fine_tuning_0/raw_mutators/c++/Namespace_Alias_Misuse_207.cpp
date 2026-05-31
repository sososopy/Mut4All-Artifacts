//header file
#pragma once
#include "Mutator_base.h"

/**
 * Namespace_Alias_Misuse_207
 */ 
class MutatorFrontendAction_207 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(207)

private:
    class MutatorASTConsumer_207 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_207(Rewriter &R) : TheRewriter(R) {}
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
        const NamespaceDecl *targetNamespace = nullptr;
        string targetNamespaceName = "";
    };
};

//source file
#include "../include/Namespace_Alias_Misuse_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      if (NS->isAnonymousNamespace())
        return;
      targetNamespace = NS;
      targetNamespaceName = NS->getNameAsString();
    } else if (auto *NG =
                   Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Guide")) {
      if (!NG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NG->getBeginLoc()))
        return;
      if (!NG->isThisDeclarationADefinition())
        return;

      if (targetNamespace == nullptr || targetNamespaceName == "")
        return;
      auto aliasname = "mut207";
      auto aliasdecl = "namespace " + aliasname + " = " + targetNamespaceName;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          NG->getSourceRange());
      llvm::outs() << decl << '\n';
      stringutils::strReplaceAll(decl, targetNamespaceName, aliasname);
      llvm::outs() << decl << '\n';
      Rewrite.ReplaceText(NG->getBeginLoc(), 0, "/*mut207*/" + aliasdecl + ";\n");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(NG->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto ns_matcher = namespaceDecl().bind("Namespace");
    auto guide_matcher = classTemplateDecl().bind("Guide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ns_matcher, &callback);
    matchFinder.addMatcher(guide_matcher, &callback);
    matchFinder.matchAST(Context);
}