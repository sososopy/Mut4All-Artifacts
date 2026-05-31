//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "llvm/Support/SourceMgr.h"

/**
 * Replace_Template_Parameter_Type_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_137>(TheRewriter);
    }

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Parameter_Type_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      if (templateParams->size() < 2)
        return;
      auto param1 = templateParams->getParam(0);
      auto param2 = templateParams->getParam(1);
      //Perform mutation on the source code text by applying string replacement
      std::string paramName1 = param1->getNameAsString();
      std::string paramName2 = param2->getNameAsString();
      std::string declaration = MT->getSourceRange().getBegin().printToString(Result.Context->getSourceManager());
      size_t pos = declaration.find(paramName1);
      if (pos != std::string::npos) {
        declaration.replace(pos, paramName1.length(), paramName2);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), declaration);
    }
}

void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(templateParameterList())).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}