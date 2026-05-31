//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_Aggregate_Initializer_With_Empty_List_212
 */ 
class MutatorFrontendAction_212 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(212)

private:
    class MutatorASTConsumer_212 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_212(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> recordDecls;
    };
};

//source file
#include "../include/Mutator_Replace_Constexpr_Aggregate_Initializer_With_Empty_List_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->isConstexpr())
        return;
      auto Init = VD->getInit();
      if (!Init)
        return;
      auto Type = VD->getType();
      if (!Type->isAggregateType() && !Type->isArrayType())
        return;
      //Get the source code text of target node
      auto VarRange = VD->getSourceRange();
      auto VarText = stringutils::rangetoStr(*(Result.SourceManager), VarRange);
      //Record the node information to be used in the mutation process
      auto RecordType = Type->getAsCXXRecordDecl();
      if (RecordType) {
        recordDecls.push_back(RecordType);
      }
      //Perform mutation on the source code text by applying string replacement
      std::string NewInit = "{}";
      size_t InitPos = VarText.find('=');
      if (InitPos != std::string::npos) {
        size_t InitEnd = VarText.find(';', InitPos);
        if (InitEnd != std::string::npos) {
          VarText.replace(InitPos + 1, InitEnd - InitPos - 1, NewInit);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarRange), VarText);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      //Check if this record is one of the types we recorded
      bool Found = false;
      for (auto Rec : recordDecls) {
        if (Rec == RD) {
          Found = true;
          break;
        }
      }
      if (!Found)
        return;
      //Get the source code text of target node
      auto RecordRange = RD->getSourceRange();
      auto RecordText = stringutils::rangetoStr(*(Result.SourceManager), RecordRange);
      //Perform mutation on the source code text by applying string replacement
      //Find constructor declarations
      bool HasConstructor = false;
      for (auto Method : RD->methods()) {
        if (Method->isUserProvided() && Method->getKind() == Decl::CXXConstructor) {
          HasConstructor = true;
          //Modify constructor to add incomplete default argument
          auto ConstructorRange = Method->getSourceRange();
          auto ConstructorText = stringutils::rangetoStr(*(Result.SourceManager), ConstructorRange);
          //Find parameter list
          size_t ParamStart = ConstructorText.find('(');
          size_t ParamEnd = ConstructorText.find(')', ParamStart);
          if (ParamStart != std::string::npos && ParamEnd != std::string::npos) {
            //Insert incomplete default argument
            ConstructorText.insert(ParamEnd, " = )");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(ConstructorRange), ConstructorText);
          }
          break;
        }
      }
      if (!HasConstructor) {
        //Add a constructor with incomplete default argument
        std::string ConstructorDecl = "\n\tconstexpr " + RD->getNameAsString() + "(const int& a = ) {}";
        size_t InsertPos = RecordText.rfind('}');
        if (InsertPos != std::string::npos) {
          RecordText.insert(InsertPos, ConstructorDecl);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(RecordRange), RecordText);
        }
      }
    }
}
  
void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl(isConstexpr(), hasInitializer(anyOf(initListExpr(), arrayInitExpr()))).bind("constexprVar");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}