//header file
#pragma once
#include "Mutator_base.h"

/**
 * Extend_Nested_Namespace_Unfinished_223
 */ 
class MutatorFrontendAction_223 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(223)

private:
    class MutatorASTConsumer_223 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_223(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/extend_nested_namespace_unfinished_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NestedNamespace")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto names = MT->getName().str();
      llvm::outs() << names << '\n';
      auto name_vec = stringutils::split(names, "::");
      string new_name = "Mut223";
      if (name_vec.size() > 0) {
        new_name = name_vec.back();
        new_name = "Mut223_" + new_name;
      }
      content = content.substr(0, content.rfind('{')) + "::" + new_name + "{";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl(hasName("::")).bind("NestedNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}