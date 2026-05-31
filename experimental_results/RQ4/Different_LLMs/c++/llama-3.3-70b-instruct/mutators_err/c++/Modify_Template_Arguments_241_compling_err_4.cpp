//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Arguments_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_241>(TheRewriter);
    }

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Arguments_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationTypeLoc>(("TemplateSpecialization"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original template argument with a non-type argument
      std::string mutatedDeclaration = declaration;
      size_t templateArgumentStart = declaration.find('<');
      size_t templateArgumentEnd = declaration.find('>');
      if (templateArgumentStart != std::string::npos && templateArgumentEnd != std::string::npos) {
        std::string templateArgument = declaration.substr(templateArgumentStart + 1, templateArgumentEnd - templateArgumentStart - 1);
        if (templateArgument.find("int") != std::string::npos || templateArgument.find("class") != std::string::npos) {
          mutatedDeclaration.replace(templateArgumentStart + 1, templateArgumentEnd - templateArgumentStart - 1, "5");
        } else {
          mutatedDeclaration.replace(templateArgumentStart + 1, templateArgumentEnd - templateArgumentStart - 1, "int");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}