//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unknown_Embed_Parameter_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)

private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::PreprocessedEntity>("EmbedDirective")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto directive = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Check if directive contains at least one valid parameter
      if (directive.find("limit") != std::string::npos || 
          directive.find("offset") != std::string::npos || 
          directive.find("if_empty") != std::string::npos) {
        // Insert unknown parameter before the first known parameter
        std::string unknown_param = "unknown_param";
        int random_value = getrandom::getRandomInt(1, 100);
        std::string insert_text = unknown_param + "(" + std::to_string(random_value) + ") ";
        
        // Find position of first known parameter
        size_t limit_pos = directive.find("limit");
        size_t offset_pos = directive.find("offset");
        size_t if_empty_pos = directive.find("if_empty");
        
        size_t insert_pos = std::string::npos;
        if (limit_pos != std::string::npos) insert_pos = limit_pos;
        if (offset_pos != std::string::npos && (insert_pos == std::string::npos || offset_pos < insert_pos)) insert_pos = offset_pos;
        if (if_empty_pos != std::string::npos && (insert_pos == std::string::npos || if_empty_pos < insert_pos)) insert_pos = if_empty_pos;
        
        if (insert_pos != std::string::npos) {
          directive.insert(insert_pos, insert_text);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), directive);
        }
      }
    }
}
  
void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = preprocessedEntity().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}