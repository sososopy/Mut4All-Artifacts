//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Template_Argument_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Template_Argument_205.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t templatePos = declaration.find("<");
      size_t endTemplatePos = declaration.find(">");
      if (templatePos != std::string::npos && endTemplatePos != std::string::npos) {
        std::string templateArgs = declaration.substr(templatePos + 1, endTemplatePos - templatePos - 1);
        // Modify template arguments by either adding, removing, or changing them
        std::vector<std::string> args;
        size_t argStart = 0;
        while (true) {
          size_t commaPos = templateArgs.find(",", argStart);
          if (commaPos == std::string::npos) {
            args.push_back(templateArgs.substr(argStart));
            break;
          }
          args.push_back(templateArgs.substr(argStart, commaPos - argStart));
          argStart = commaPos + 1;
        }
        // Randomly modify template arguments
        int choice = getrandom::getRandomIndex(3);
        if (choice == 0) { // Add a new argument
          args.push_back("int");
        } else if (choice == 1) { // Remove an existing argument
          if (!args.empty()) {
            args.erase(args.begin());
          }
        } else { // Change an existing argument
          if (!args.empty()) {
            args[0] = "float";
          }
        }
        std::string newTemplateArgs;
        for (const auto &arg : args) {
          newTemplateArgs += arg + ",";
        }
        if (!newTemplateArgs.empty()) {
          newTemplateArgs.pop_back(); // Remove the trailing comma
        }
        mutatedDeclaration.replace(templatePos + 1, endTemplatePos - templatePos - 1, newTemplateArgs);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}