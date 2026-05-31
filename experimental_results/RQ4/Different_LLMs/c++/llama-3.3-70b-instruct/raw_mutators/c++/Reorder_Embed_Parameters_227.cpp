//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reorder_Embed_Parameters_227
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
#include "../include/Mutator_Reorder_Embed_Parameters_227.h"

// ========================================================================================================
#define MUT227_OUTPUT 1

void MutatorFrontendAction_227::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::PreprocessorDirective>(("EmbedDirective"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto directive = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = directive.find("#embed");
      if (pos != string::npos) {
        size_t start = directive.find_first_of("(", pos);
        size_t end = directive.find_first_of(")", pos);
        if (start != string::npos && end != string::npos) {
          string params = directive.substr(start + 1, end - start - 1);
          vector<string> paramList;
          size_t pos2 = params.find(",");
          while (pos2 != string::npos) {
            paramList.push_back(params.substr(0, pos2));
            params = params.substr(pos2 + 1);
            pos2 = params.find(",");
          }
          paramList.push_back(params);
          string knownParams[] = {"limit", "offset"};
          vector<string> knownList;
          vector<string> unknownList;
          for (string param : paramList) {
            bool isKnown = false;
            for (string knownParam : knownParams) {
              if (param.find(knownParam) != string::npos) {
                isKnown = true;
                break;
              }
            }
            if (isKnown) {
              knownList.push_back(param);
            } else {
              unknownList.push_back(param);
            }
          }
          if (!unknownList.empty() && !knownList.empty()) {
            string mutatedParams;
            for (string param : unknownList) {
              mutatedParams += param + ", ";
            }
            for (string param : knownList) {
              mutatedParams += param + ", ";
            }
            mutatedParams = mutatedParams.substr(0, mutatedParams.size() - 2);
            string mutatedDirective = directive.substr(0, start + 1) + mutatedParams + directive.substr(end);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDirective);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_227::MutatorASTConsumer_227::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = preprocessorDirective(hasDirectiveText("#embed")).bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}