//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Nesting_Duplication_390
 */ 
class MutatorFrontendAction_390 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(390)

private:
    class MutatorASTConsumer_390 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_390(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamespaceDecl*> candidateNamespaces;
    };
};

//source file
#include "../include/Inline_Namespace_Nesting_Duplication_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Check if namespace is nested (has a parent namespace)
      if (!NS->getParent() || !clang::isa<clang::NamespaceDecl>(NS->getParent()))
        return;
      //Record candidate namespace
      candidateNamespaces.push_back(NS);
    }
    else if (auto *PNS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("ParentNamespace")) {
      //Filter nodes in header files
      if (!PNS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PNS->getLocation()))
        return;
      //Find nested namespace declarations within this parent
      for (auto *NS : candidateNamespaces) {
        if (NS->getParent() == PNS) {
          //Get the source code text of target node
          std::string nsName = NS->getNameAsString();
          SourceLocation nsLoc = NS->getBeginLoc();
          bool isInline = NS->isInline();
          
          //Perform mutation on the source code text by applying string replacement
          std::string mutation;
          if (!isInline) {
            //Add inline keyword to existing namespace
            mutation = "inline namespace " + nsName + " {\n";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()), mutation);
          } else {
            //Create duplicate inline namespace adjacent to existing one
            mutation = "\ninline namespace " + nsName + " {\n}\n";
            //Insert after the existing namespace declaration
            Rewrite.InsertTextAfterToken(NS->getEndLoc(), mutation);
          }
          //Replace the original AST node with the mutated one
          //Mutation applied above via ReplaceText or InsertTextAfterToken
        }
      }
    }
}
  
void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher nestedMatcher = namespaceDecl(hasParent(namespaceDecl())).bind("NestedNamespace");
    DeclarationMatcher parentMatcher = namespaceDecl().bind("ParentNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.addMatcher(parentMatcher, &callback);
    matchFinder.matchAST(Context);
}