//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Nested_Namespace_283
 */ 
class MutatorFrontendAction_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(283)

private:
    class MutatorASTConsumer_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_283(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamespaceDecl *> nested_namespaces;
    };
};

//source file
#include "../include/incomplete_nested_namespace_283.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      if (NS->isAnonymousNamespace())
        return;
      if (NS->isInlineNamespace())
        return;
      if (NS->isNested())
        nested_namespaces.push_back(NS);
    } else if (auto *OS =
                   Result.Nodes.getNodeAs<clang::NamespaceDecl>("Outer")) {
      if (!OS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     OS->getLocation()))
        return;
      if (nested_namespaces.empty())
        return;
      auto target = nested_namespaces[getrandom::getRandomIndex(
          nested_namespaces.size() - 1)];
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             OS->getSourceRange());
      auto target_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    target->getSourceRange());
      llvm::outs() << content << '\n';
      llvm::outs() << target_content << '\n';
      if (target_content.rfind('}') != string::npos)
        target_content.erase(target_content.rfind('}'));
      llvm::outs() << target_content << '\n';
      content.replace(content.find(target_content), target_content.length(),
                      target_content);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(OS->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = namespaceDecl().bind("Namespaces");
    auto outer_matcher =
        namespaceDecl(hasDescendant(namespaceDecl())).bind("Outer");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(outer_matcher, &callback);
    matchFinder.matchAST(Context);
}