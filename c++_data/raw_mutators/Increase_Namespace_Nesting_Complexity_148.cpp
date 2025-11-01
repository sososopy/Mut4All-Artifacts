//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increase_Namespace_Nesting_Complexity_148
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
    };
};

//source file
#include "../include/Increase_Namespace_Nesting_Complexity_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      //Filter nodes in header files
      if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NSD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto namespaceContent = stringutils::rangetoStr(*(Result.SourceManager),
                                                      NSD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string newNamespaceName = "NestedLevel";
      std::string newNamespace = "namespace " + newNamespaceName + " {\n";
      namespaceContent.insert(namespaceContent.find("{") + 1, "\n" + newNamespace);
      namespaceContent.append("\n} // End of NestedLevel\n");

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NSD->getSourceRange()), namespaceContent);
    }
}
  
void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}