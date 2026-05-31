//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Token_Into_Annotation_266
 */ 
class MutatorFrontendAction_266 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(266)

private:
    class MutatorASTConsumer_266 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_266(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTokens;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AD = Result.Nodes.getNodeAs<clang::AnnotateAttr>("Annotation")) {
      //Filter nodes in header files
      if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AD->getLocation()))
        return;
      //Get the source code text of target node
      SourceRange range = AD->getRange();
      std::string annotationText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Collect available tokens from the AST context
      if (availableTokens.empty()) {
        ASTContext *ctx = Result.Context;
        for (auto *decl : ctx->getTranslationUnitDecl()->decls()) {
          if (auto *namedDecl = dyn_cast<NamedDecl>(decl)) {
            std::1string name = namedDecl->getNameAsString();
            if (!name.empty()) {
              availableTokens.push_back(name);
            }
          }
        }
        //Add some keywords and punctuation as fallback
        availableTokens.push_back("int");
        availableTokens.push_back("void");
        availableTokens.push_back(";");
        availableTokens.push_back("}");
      }
      
      //Choose a token to insert
      size_t tokenIndex = getrandom::getRandomIndex(availableTokens.size() - 1);
      std::string tokenToInsert = availableTokens[tokenIndex];
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = annotationText + " " + tokenToInsert;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
    }
}
  
void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = annotateAttr().bind("Annotation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(mat1cher, &callback);
    matchFinder.matchAST(Context);
}