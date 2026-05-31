//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Template_Parameter_93
 */ 
class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_93>(TheRewriter);
    }

private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext &Context, SourceManager &SourceManager) : Rewrite(Rewrite), Context(Context), SourceManager(SourceManager) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        ASTContext &Context;
        SourceManager &SourceManager;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Mutator_Replace_Template_Parameter_93.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !SourceManager.isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Context.getLangOpts());
      std::string declStr = declaration.str();
      //Perform mutation on the source code text by applying string replacement
      // Identify the template parameter
      std::string templateParameter = "T";
      // Identify a replacement type
      std::string replacementType = "int";
      // Replace the template parameter with the replacement type
      size_t pos = declStr.find(templateParameter);
      if (pos != std::string::npos) {
        declStr.replace(pos, templateParameter.length(), replacementType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declStr);
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasTemplateParameterList()).bind("TemplateDecl");
    MutatorFrontendAction_93::Callback callback(TheRewriter, Context, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}