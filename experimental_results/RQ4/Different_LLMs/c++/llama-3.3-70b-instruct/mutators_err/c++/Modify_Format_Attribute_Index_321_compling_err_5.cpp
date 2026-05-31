//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Format_Attribute_Index_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_321>(TheRewriter);
    }

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Format_Attribute_Index_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto attrs = FD->attrs();
      for (auto attr : attrs) {
        if (auto formatAttr = dyn_cast<clang::FormatAttr>(attr)) {
          int paramCount = FD->getNumParams();
          int newIndex = -1;
          if (paramCount > 0) {
            newIndex = paramCount + 1; // exceed the number of parameters
          } else {
            newIndex = 0; // set to 0 if the function has no parameters
          }
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedAttr = "__attribute__((__format_arg__(" + std::to_string(newIndex) + ")))";
          SourceLocation start = formatAttr->getLoc();
          SourceLocation end = formatAttr->getLoc().getLocWithOffset(formatAttr->getSourceRange().getEnd().getOffset() - formatAttr->getSourceRange().getBegin().getOffset());
          Rewrite.ReplaceText(SourceRange(start, end), mutatedAttr);
        }
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(decl(has(attr::FormatAttr())))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}