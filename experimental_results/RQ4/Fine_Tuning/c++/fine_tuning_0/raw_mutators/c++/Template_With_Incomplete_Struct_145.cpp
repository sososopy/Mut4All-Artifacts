//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_With_Incomplete_Struct_145
 */ 
class MutatorFrontendAction_145 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(145)

private:
    class MutatorASTConsumer_145 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_145(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_With_Incomplete_Struct_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto template_name = MT->getNameAsString();
      auto template_range = MT->getSourceRange();
      auto template_content = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << template_content;
      auto instantiations = Result.Nodes.getNodeAs<clang::CallExpr>(
          "TemplateInstantiations");
      if (!instantiations)
        return;
      auto instantiation_content = stringutils::rangetoStr(
          *(Result.SourceManager), instantiations->getSourceRange());
      llvm::outs() << instantiation_content;
      //Perform mutation on the source code text by applying string replacement
      if (instantiation_content.find(template_name) ==
          string::npos) // not instantiation
        return;
      string struct_declaration = "struct B;\n";
      string instantiation_with_incomplete_struct = "B";
      string replacement = "/*mut145*/" + struct_declaration +
                           template_content + instantiation_content;
      llvm::outs() << replacement;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), 0, replacement);
    }
}
  
void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namedDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}