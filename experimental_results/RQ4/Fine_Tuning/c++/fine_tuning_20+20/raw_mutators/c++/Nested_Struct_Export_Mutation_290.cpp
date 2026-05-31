//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Struct_Export_Mutation_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/nested_struct_export_mutation_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ExportDecl>("Exports")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("export struct") != string::npos) {
        llvm::outs() << content << '\n';
        content.replace(content.find("export struct"), 6, "");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = exportDecl().bind("Exports");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}