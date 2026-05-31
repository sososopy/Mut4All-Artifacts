//header file
#pragma once
#include "Mutator_base.h"

/**
 * Manipulate_Namespace_Hierarchy_148
 */ 
class MutatorFrontendAction_148 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(148)

private:
    class MutatorASTConsumer_148 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_148(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamespaceDecl *> namespaces;
    };
};

//source file
#include "../include/Manipulate_Namespace_Hierarchy_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      namespaces.push_back(NS);
    } else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>(
                   "ManipulateNamespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      auto name = NS->getNameAsString();
      if (name == "")
        return;
      if (name == "__1" || name == "__cxx11")
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), NS->getSourceRange());
      llvm::outs() << content;
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        content = "namespace Mut_148 {\n" + content + "\n}";
        llvm::outs() << content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()),
                            content);
      } else {
        content = content.substr(content.find('{') + 1,
                                 content.rfind('}') - content.find('{') - 1);
        llvm::outs() << content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()),
                            content);
      }
    }
  }
  
void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = namespaceDecl().bind("Namespaces");
    auto manipulate_matcher = namespaceDecl().bind("ManipulateNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder matchFinder2;
    matchFinder2.addMatcher(manipulate_matcher, &callback);
    matchFinder2.matchAST(Context);
}