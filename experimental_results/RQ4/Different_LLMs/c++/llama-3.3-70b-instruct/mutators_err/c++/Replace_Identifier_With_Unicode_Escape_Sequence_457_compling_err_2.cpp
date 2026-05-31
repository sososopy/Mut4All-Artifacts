//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Identifier_With_Unicode_Escape_Sequence_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_457>(TheRewriter);
    }

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Identifier_With_Unicode_Escape_Sequence_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangeToStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string identifier = MT->getNameAsString();
      std::string unicodeEscapeSequence;
      for (char c : identifier) {
        unicodeEscapeSequence += "\\u" + std::to_string((int)c);
      }
      declaration.replace(declaration.find(identifier), identifier.length(), unicodeEscapeSequence);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}