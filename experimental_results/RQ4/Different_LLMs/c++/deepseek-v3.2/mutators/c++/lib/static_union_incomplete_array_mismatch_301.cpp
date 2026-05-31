//source file
#include "../include/Static_Union_Incomplete_Array_Mismatch_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("StaticUnionVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if variable is static
      if (!VD->isStaticLocal()) return;
      
      // Check if type is union
      auto QT = VD->getType();
      if (!QT->isUnionType()) return;
      
      // Get union declaration
      const RecordDecl* RD = QT->getAsRecordDecl();
      if (!RD) return;
      
      // Check if union has at least one member with incomplete array type
      bool hasIncompleteArray = false;
      for (auto Field : RD->fields()) {
        if (Field->getType()->isIncompleteArrayType()) {
          hasIncompleteArray = true;
          break;
        }
      }
      if (!hasIncompleteArray) return;
      
      // Get the source code text of target node
      auto initRange = VD->getInit();
      if (!initRange) return;
      
      // Collect available fundamental types from the seed program
      availableTypes.clear();
      auto& AST = *Result.Context;
      for (auto Decl : AST.getTranslationUnitDecl()->decls()) {
        if (auto FD = dyn_cast<FunctionDecl>(Decl)) {
          for (auto Param : FD->parameters()) {
            QualType PT = Param->getType();
            if (PT->isFundamentalType() || PT->isPointerType()) {
              availableTypes.push_back(PT);
            }
          }
        }
        if (auto VD2 = dyn_cast<VarDecl>(Decl)) {
          QualType VT = VD2->getType();
          if (VT->isFundamentalType() || VT->isPointerType()) {
            availableTypes.push_back(VT);
          }
        }
      }
      
      if (availableTypes.empty()) return;
      
      // Choose a mismatching type
      size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
      QualType chosenType = availableTypes[idx];
      
      // Generate appropriate constant initializer
      std::string initValue;
      if (chosenType->isPointerType()) {
        initValue = "0";
      } else if (chosenType->isFloatingType()) {
        initValue = "1.0";
      } else if (chosenType->isIntegerType()) {
        initValue = "1";
      } else if (chosenType->isCharType()) {
        initValue = "'a'";
      } else {
        initValue = "0";
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string newInit = "{ " + initValue + " }";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange->getSourceRange()), newInit);
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isStaticLocal(), hasType(recordDecl(isUnion()))).bind("StaticUnionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}