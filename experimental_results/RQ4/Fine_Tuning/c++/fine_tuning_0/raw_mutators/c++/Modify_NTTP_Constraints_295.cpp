//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_NTTP_Constraints_295
 */ 
class MutatorFrontendAction_295 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(295)

private:
    class MutatorASTConsumer_295 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_295(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_NTTP_Constraints_295.h"

// ========================================================================================================
#define MUT295_OUTPUT 1

void MutatorFrontendAction_295::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("NTTP")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getTypeSourceInfo()->getTypeLoc());
      llvm::outs() << type << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (type == "int") {
        text = std::regex_replace(text, std::regex("int"), "long long");
      } else if (type == "long long") {
        text = std::regex_replace(text, std::regex("long long"), "int");
      } else if (type == "char") {
        text = std::regex_replace(text, std::regex("char"), "int");
      } else if (type == "bool") {
        text = std::regex_replace(text, std::regex("bool"), "int");
      } else if (type == "float") {
        text = std::regex_replace(text, std::regex("float"), "double");
      } else if (type == "double") {
        text = std::regex_replace(text, std::regex("double"), "float");
      } else if (type == "long double") {
        text = std::regex_replace(text, std::regex("long double"), "double");
      } else if (type == "unsigned int") {
        text = std::regex_replace(text, std::regex("unsigned int"), "int");
      } else if (type == "unsigned long long") {
        text =
            std::regex_replace(text, std::regex("unsigned long long"), "int");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), text);
    }
}
  
void MutatorFrontendAction_295::MutatorASTConsumer_295::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = nonTypeTemplateParmDecl().bind("NTTP");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}