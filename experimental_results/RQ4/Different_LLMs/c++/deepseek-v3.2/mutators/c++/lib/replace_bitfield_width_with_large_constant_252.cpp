//source file
#include "../include/Mutator_Replace_Bitfield_Width_With_Large_Constant_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Bitfield")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a bitfield
      if (!FD->isBitField())
        return;
      
      //Get the source code text of target node
      const Expr* WidthExpr = FD->getBitWidth();
      if (!WidthExpr)
        return;
      
      SourceRange WidthRange = WidthExpr->getSourceRange();
      if (WidthRange.isInvalid())
        return;
      
      //Collect large constants from the translation unit
      if (largeConstants.empty()) {
        ASTContext& Context = *Result.Context;
        TranslationUnitDecl* TU = Context.getTranslationUnitDecl();
        for (Decl* D : TU->decls()) {
          if (VarDecl* VD = dyn_cast<VarDecl>(D)) {
            if (VD->getType()->isIntegerType() && VD->hasGlobalStorage()) {
              if (VD->getEvaluatedValue() && VD->getEvaluatedValue()->isInt()) {
                llvm::APSInt Val = VD->getEvaluatedValue()->getInt();
                if (Val.getBitWidth() >= 64 && Val > 1000) {
                  largeConstants.push_back(VD->getNameAsString());
                }
              }
            }
          } else if (EnumDecl* ED = dyn_cast<EnumDecl>(D)) {
            for (EnumConstantDecl* ECD : ED->enumerators()) {
              if (ECD->getInitVal().getBitWidth() >= 64 && ECD->getInitVal() > 1000) {
                largeConstants.push_back(ECD->getNameAsString());
              }
            }
          }
        }
        //Add ULLONG_MAX if not already present
        if (largeConstants.empty()) {
          largeConstants.push_back("ULLONG_MAX");
        }
      }
      
      //Select a large constant
      std::string replacement;
      if (!largeConstants.empty()) {
        size_t idx = getrandom::getRandomIndex(largeConstants.size() - 1);
        replacement = largeConstants[idx];
      } else {
        replacement = "18446744073709551615ULL"; //ULLONG_MAX literal
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(WidthRange, replacement);
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(isBitField()).bind("Bitfield");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}