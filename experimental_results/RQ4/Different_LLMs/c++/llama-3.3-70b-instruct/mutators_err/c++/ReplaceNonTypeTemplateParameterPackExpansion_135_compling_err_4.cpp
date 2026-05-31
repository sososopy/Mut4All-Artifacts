//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceNonTypeTemplateParameterPackExpansion_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_135>(TheRewriter);
    }

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceNonTypeTemplateParameterPackExpansion_135.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateParameterList")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getLocStart().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto declaration = Lexer::getSourceText(CharSourceRange::getTokenRange(MT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      std::string declStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      // Replace the expansion of a non-type template parameter pack with a single non-type template parameter or vice versa
      if (declStr.find("...") != std::string::npos) {
        // Replace pack expansion with a single non-type template parameter
        declStr.replace(declStr.find("..."), 3, "");
      } else {
        // Replace single non-type template parameter with pack expansion
        declStr.insert(declStr.find(">"), "...");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declStr);
    }
}
  
void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasTemplateParameterList(hasAnyTemplateParameter(isTemplateParameterPack())));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}