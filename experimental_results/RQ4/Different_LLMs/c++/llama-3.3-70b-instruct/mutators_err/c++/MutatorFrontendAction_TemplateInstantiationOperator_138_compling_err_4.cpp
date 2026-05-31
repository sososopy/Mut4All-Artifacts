//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationOperator_138
 */ 
class MutatorFrontendAction_TemplateInstantiationOperator_138 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_TemplateInstantiationOperator_138>(TheRewriter);
    }

private:
    class MutatorASTConsumer_TemplateInstantiationOperator_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationOperator_138(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationOperator_138.h"
#include <regex>
#include <string>

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationOperator_138::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Randomly change the template arguments
      std::string mutatedDeclaration = declaration;
      std::regex templateParamRegex("<([^>]+)>");
      auto words_begin = std::sregex_iterator(mutatedDeclaration.begin(), mutatedDeclaration.end(), templateParamRegex);
      auto words_end = std::sregex_iterator();
      for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string templateParam = match.str(1); 
        // Randomly change the template parameter
        if (templateParam.find("int") != std::string::npos) {
          mutatedDeclaration.replace(match.position() + 1, match.length() - 2, "float");
        } else if (templateParam.find("float") != std::string::npos) {
          mutatedDeclaration.replace(match.position() + 1, match.length() - 2, "double");
        } else if (templateParam.find("double") != std::string::npos) {
          mutatedDeclaration.replace(match.position() + 1, match.length() - 2, "int");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationOperator_138::MutatorASTConsumer_TemplateInstantiationOperator_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}