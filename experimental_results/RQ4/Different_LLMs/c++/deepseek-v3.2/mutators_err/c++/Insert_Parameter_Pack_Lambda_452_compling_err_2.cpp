//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Parameter_Pack_Lambda_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Parameter_Pack_Lambda_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template function with parameter pack
      if (!FD->isTemplated() || !FD->getTemplateSpecializationInfo())
        return;
      const TemplateArgumentList *TAL = FD->getTemplateSpecializationInfo()->TemplateArguments;
      if (!TAL || TAL->size() == 0)
        return;
      bool hasParameterPack = false;
      for (unsigned i = 0; i < TAL->size(); ++i) {
        if (TAL->get(i).getKind() == TemplateArgument::Pack)
          hasParameterPack = true;
      }
      if (!hasParameterPack)
        return;
      //Get the source code text of target node
      Stmt *Body = FD->getBody();
      if (!Body)
        return;
      //Find the first parameter pack in function parameters
      std::string OuterPackName;
      for (auto *P : FD->parameters()) {
        if (P->getType()->getContainedAutoType() || P->getType()->isTemplateTypeParmType()) {
          OuterPackName = P->getNameAsString();
          break;
        }
      }
      if (OuterPackName.empty())
        return;
      //Generate unique inner pack name
      std::string InnerPackName = "__mut_pack";
      //Check if body is compound statement, if not wrap it
      std::string BodyText;
      SourceRange BodyRange;
      if (isa<CompoundStmt>(Body)) {
        BodyText = stringutils::rangetoStr(*(Result.SourceManager), Body->getSourceRange());
        BodyRange = Body->getSourceRange();
      } else {
        //Wrap single statement or empty body in braces
        SourceLocation StartLoc = FD->getBody()->getBeginLoc();
        SourceLocation EndLoc = FD->getBody()->getEndLoc();
        BodyText = "{ " + stringutils::rangetoStr(*(Result.SourceManager), SourceRange(StartLoc, EndLoc)) + " }";
        BodyRange = SourceRange(StartLoc, EndLoc);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string Insertion = "\n(void)[&](auto... " + InnerPackName + ") { ([&] { " + OuterPackName + "; " + InnerPackName + "; }, ...); };";
      //Insert before the closing brace
      size_t InsertPos = BodyText.rfind('}');
      if (InsertPos != std::string::npos) {
        BodyText.insert(InsertPos, Insertion);
      } else {
        BodyText += Insertion;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BodyRange), BodyText);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplated(), hasBody(anything())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}