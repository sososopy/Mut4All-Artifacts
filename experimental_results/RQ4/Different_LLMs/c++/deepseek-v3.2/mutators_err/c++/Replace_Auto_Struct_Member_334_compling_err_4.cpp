//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Struct_Member_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if member is declared with auto
      if (!FD->getTypeSourceInfo() || !FD->getTypeSourceInfo()->getType()->getAs<clang::AutoType>())
        return;
      
      // Check if member is non-static
      if (FD->isStaticDataMember())
        return;
      
      // Check if member has an initializer
      const Expr *Init = FD->getInClassInitializer();
      if (!Init)
        return;
      
      // Check if initializer is a simple literal or constant expression
      // We'll check for integer, floating-point, string literals, and simple constant expressions
      bool isSimpleLiteral = false;
      if (isa<IntegerLiteral>(Init) || 
          isa<FloatingLiteral>(Init) || 
          isa<CharacterLiteral>(Init) || 
          isa<CXXBoolLiteralExpr>(Init)) {
        isSimpleLiteral = true;
      }
      
      // Also check for simple constant expressions like nullptr
      if (isa<CXXNullPtrLiteralExpr>(Init))
        isSimpleLiteral = true;
      
      // Check for string literals
      if (isa<StringLiteral>(Init))
        isSimpleLiteral = true;
      
      // Check for simple arithmetic constant expressions
      if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(Init)) {
        if (BO->isAdditiveOp() || BO->isMultiplicativeOp()) {
          // Check if both operands are literals
          if (isa<IntegerLiteral>(BO->getLHS()) && isa<IntegerLiteral>(BO->getRHS()))
            isSimpleLiteral = true;
        }
      }
      
      if (!isSimpleLiteral)
        return;
      
      // Get the deduced type from the initializer
      QualType DeducedType = FD->getType();
      if (DeducedType.isNull())
        return;
      
      // Get the type as string
      std::string TypeStr = DeducedType.getAsString();
      
      // Get the source code text of target node
      std::string FieldText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      
      // Find the "auto" keyword and replace it with the deduced type
      size_t autoPos = FieldText.find("auto");
      if (autoPos == std::string::npos)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      FieldText.replace(autoPos, 4, TypeStr);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), FieldText);
    }
}
  
void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(anything()), unless(isStaticDataMember()), hasType(autoType())).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}