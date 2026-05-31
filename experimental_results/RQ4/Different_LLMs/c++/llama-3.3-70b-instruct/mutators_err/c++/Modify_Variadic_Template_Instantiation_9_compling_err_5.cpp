//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variadic_Template_Instantiation_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_9>(TheRewriter);
    }

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Variadic_Template_Instantiation_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getLocStart(), MT->getLocEnd());
      //Perform mutation on the source code text by applying string replacement
      //Example: Replace A<short, int> with A<int, short, float>
      std::string mutatedDeclaration = declaration;
      size_t templateStart = declaration.find('<');
      size_t templateEnd = declaration.find('>');
      if (templateStart != std::string::npos && templateEnd != std::string::npos) {
        std::string templateArgs = declaration.substr(templateStart + 1, templateEnd - templateStart - 1);
        //Add, remove, or reorder template arguments
        std::vector<std::string> args;
        size_t argStart = 0;
        while (true) {
          size_t argEnd = templateArgs.find(',', argStart);
          if (argEnd == std::string::npos) {
            args.push_back(templateArgs.substr(argStart));
            break;
          }
          args.push_back(templateArgs.substr(argStart, argEnd - argStart));
          argStart = argEnd + 1;
        }
        //Example mutation: add a new argument
        args.push_back("float");
        //Reorder arguments
        std::random_shuffle(args.begin(), args.end());
        std::string newTemplateArgs;
        for (const auto &arg : args) {
          if (!newTemplateArgs.empty()) {
            newTemplateArgs += ", ";
          }
          newTemplateArgs += arg;
        }
        mutatedDeclaration.replace(templateStart + 1, templateEnd - templateStart - 1, newTemplateArgs);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getLocStart(), MT->getLocEnd()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}