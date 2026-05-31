//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Arguments_369
 */ 
class MutatorFrontendAction_369 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_369>(TheRewriter);
    }

private:
    class MutatorASTConsumer_369 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_369(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Arguments_369.h"

// ========================================================================================================
#define MUT369_OUTPUT 1

void MutatorFrontendAction_369::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getAsTypeLoc().getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getAsTypeLoc().getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string modifiedDeclaration = declaration;
      size_t pos = modifiedDeclaration.find('<');
      if (pos != std::string::npos) {
        size_t endPos = modifiedDeclaration.find('>');
        if (endPos != std::string::npos) {
          std::string templateArgs = modifiedDeclaration.substr(pos + 1, endPos - pos - 1);
          std::vector<std::string> args;
          size_t argPos = 0;
          while ((argPos = templateArgs.find(',')) != std::string::npos) {
            args.push_back(templateArgs.substr(0, argPos));
            templateArgs.erase(0, argPos + 1);
          }
          args.push_back(templateArgs);
          if (!args.empty()) {
            // Replace one of the template arguments with a different type
            int choice = getrandom::getRandomIndex(args.size());
            args[choice] = "int*"; // Replace with a pointer type
            std::string newTemplateArgs;
            for (const auto &arg : args) {
              newTemplateArgs += arg + ",";
            }
            newTemplateArgs.pop_back(); // Remove the trailing comma
            modifiedDeclaration.replace(pos + 1, endPos - pos - 1, newTemplateArgs);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getAsTypeLoc().getBeginLoc(), MT->getAsTypeLoc().getEndLoc()), modifiedDeclaration);
    }
}
  
void MutatorFrontendAction_369::MutatorASTConsumer_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}