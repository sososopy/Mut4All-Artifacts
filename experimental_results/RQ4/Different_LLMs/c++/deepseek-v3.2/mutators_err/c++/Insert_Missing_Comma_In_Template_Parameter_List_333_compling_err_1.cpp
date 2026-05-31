//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Missing_Comma_In_Template_Parameter_List_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      const TemplateParameterList *TPL = TD->getTemplateParameters();
      if (!TPL || TPL->size() < 2)
        return;
      
      SourceManager &SM = *Result.SourceManager;
      const LangOptions &LangOpts = Result.Context->getLangOpts();
      
      //Iterate through template parameters to find a pack followed by another parameter without comma
      for (unsigned i = 0; i < TPL->size() - 1; ++i) {
        const NamedDecl *Param1 = TPL->getParam(i);
        const NamedDecl *Param2 = TPL->getParam(i + 1);
        
        //Check if Param1 is a template parameter pack
        if (!Param1->isTemplateParameterPack())
          continue;
        
        //Get source ranges
        SourceRange Range1 = Param1->getSourceRange();
        SourceRange Range2 = Param2->getSourceRange();
        
        if (!Range1.isValid() || !Range2.isValid())
          continue;
        
        //Get the text between the two parameters
        SourceLocation End1 = Range1.getEnd();
        SourceLocation Begin2 = Range2.getBegin();
        
        //Check if there's a comma token between them
        bool hasComma = false;
        Token Tok;
        if (Lexer::getRawToken(End1, Tok, SM, LangOpts, false)) {
          //Lexer failed, skip
          continue;
        }
        if (Tok.getKind() == tok::comma) {
          hasComma = true;
        }
        
        //If no comma, insert one after Param1's ellipsis
        if (!hasComma) {
          //Find the ellipsis location in Param1
          std::string Param1Text = stringutils::rangetoStr(SM, Range1);
          size_t ellipsisPos = Param1Text.find("...");
          if (ellipsisPos == std::string::npos)
            continue;
          
          //Calculate insertion location: after the ellipsis token
          SourceLocation EllipsisLoc = Range1.getBegin().getLocWithOffset(ellipsisPos);
          SourceLocation AfterEllipsis = Lexer::getLocForEndOfToken(EllipsisLoc, 0, SM, LangOpts);
          
          //Insert comma
          Rewrite.InsertTextAfterToken(AfterEllipsis, ",");
          return; //Perform one mutation per template declaration
        }
      }
    }
}
  
void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}