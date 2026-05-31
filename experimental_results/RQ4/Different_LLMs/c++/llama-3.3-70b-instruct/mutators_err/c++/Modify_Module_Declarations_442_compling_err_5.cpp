//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Module_Declarations_442
 */ 
class MutatorFrontendAction_442 : public MutatorFrontendAction {
public:
    MutatorFrontendAction_442() {}
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_442>(TheRewriter);
    }

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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Module_Declarations_442.h"
#include "clang/AST/Decl.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ModuleDecl>("module")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto module_name = MT->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto new_module_name = module_name + "_modified";
      auto declaration = "export module " + new_module_name + ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::decl(clang::ast_matchers::anyOf(
        clang::ast_matchers::module()
    )).bind("module");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}