//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Macro_Invocation_Context_283
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Macro_Invocation_Context_283.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MI = Result.Nodes.getNodeAs<clang::MacroInfo>("MacroInvocations")) {
      //Filter nodes in header files
      if (!MI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MI->getDefinitionLoc()))
        return;
      //Get the source code text of target node
      auto macro_name = MI->getName();
      auto macro_loc = MI->getDefinitionLoc();
      auto macro_range = MI->getSourceRange();
      auto macro_text = stringutils::rangetoStr(*(Result.SourceManager), macro_range);
      llvm::outs() << "Macro name: " << macro_name << '\n';
      llvm::outs() << "Macro location: " << macro_loc.printToString(*(Result.SourceManager)) << '\n';
      llvm::outs() << "Macro text: " << macro_text << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto file_end_loc = Result.SourceManager->getLocForEndOfFile(Result.SourceManager->getMainFileID());
      auto file_end_text = stringutils::rangetoStr(*(Result.SourceManager), SourceRange(file_end_loc, file_end_loc));
      llvm::outs() << "File end text: " << file_end_text << '\n';
      if (file_end_text.back() == '\n') {
        llvm::outs() << "File end with newline\n";
        Rewrite.ReplaceText(file_end_loc, 0, "/*mut283*/" + macro_text);
      } else {
        llvm::outs() << "File end without newline\n";
        Rewrite.ReplaceText(file_end_loc, 0, "/*mut283*/" + macro_text + "\n");
      }
    }
}
  
void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::macroDefinition().bind("MacroInvocations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}