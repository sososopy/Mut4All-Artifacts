//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Deduced_Type_242
 */ 
class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingPointerTypes;
    };
};

//source file
#include "../include/Mutator_Replace_Deduced_Type_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("decltypeAutoVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check that the variable is declared with decltype(auto)
      if (!VD->getTypeSourceInfo() || 
          !VD->getTypeSourceInfo()->getTypeLoc().getType()->isDecltypeAuto())
        return;
      
      // Check that the initializer is a pointer to a function template
      const Expr* InitExpr = VD->getInit();
      if (!InitExpr)
        return;
      
      // Check if it's a pointer to a function (address of function)
      InitExpr = InitExpr->IgnoreParens();
      if (!isa<UnaryOperator>(InitExpr) || 
          InitExpr->getStmtClass() != Stmt::UnaryOperatorClass)
        return;
      
      const UnaryOperator* UO = dyn_cast<UnaryOperator>(InitExpr);
      if (UO->getOpcode() != UO_AddrOf)
        return;
      
      const Expr* SubExpr = UO->getSubExpr()->IgnoreParens();
      if (!isa<DeclRefExpr>(SubExpr))
        return;
      
      const DeclRefExpr* DRE = dyn_cast<DeclRefExpr>(SubExpr);
      const ValueDecl* ReferencedDecl = DRE->getDecl();
      if (!ReferencedDecl || !isa<FunctionDecl>(ReferencedDecl))
        return;
      
      const FunctionDecl* FD = dyn_cast<FunctionDecl>(ReferencedDecl);
      // Check if it's a template function
      if (FD->getTemplatedKind() != FunctionDecl::TK_FunctionTemplate)
        return;
      
      //Get the source code text of target node
      SourceRange VarDeclRange = VD->getSourceRange();
      std::string VarDeclText = stringutils::rangetoStr(*(Result.SourceManager), VarDeclRange);
      
      // Find the position of "decltype(auto)"
      size_t decltypePos = VarDeclText.find("decltype(auto)");
      if (decltypePos == std::string::npos)
        return;
      
      // Determine replacement type
      std::string replacementType;
      if (existingPointerTypes.empty()) {
        // Fallback to "auto"
        replacementType = "auto";
      } else {
        // Choose a random existing pointer type
        size_t index = getrandom::getRandomIndex(existingPointerTypes.size() - 1);
        replacementType = existingPointerTypes[index];
      }
      
      //Perform mutation on the source code text by applying string replacement
      VarDeclText.replace(decltypePos, 14, replacementType);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarDeclRange), VarDeclText);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("pointerVar")) {
      // Collect existing pointer types from the seed program
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      const Type* VarType = VD->getType().getTypePtr();
      if (VarType->isPointerType()) {
        // Get the source text of the type
        if (VD->getTypeSourceInfo()) {
          TypeLoc TL = VD->getTypeSourceInfo()->getTypeLoc();
          std::string TypeText = stringutils::rangetoStr(*(Result.SourceManager), TL.getSourceRange());
          existingPointerTypes.push_back(TypeText);
        }
      }
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher decltypeAutoMatcher = varDecl(hasType(hasCanonicalType(decltypeAutoType())), hasInitializer(hasDescendant(unaryOperator(hasOperatorName("&"))))).bind("decltypeAutoVar");
    DeclarationMatcher pointerVarMatcher = varDecl(hasType(pointerType())).bind("pointerVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decltypeAutoMatcher, &callback);
    matchFinder.addMatcher(pointerVarMatcher, &callback);
    matchFinder.matchAST(Context);
}