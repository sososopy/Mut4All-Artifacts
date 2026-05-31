//source file
#include "../include/Mutator_Incomplete_Type_Enum_Mutation_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("incompleteType")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Record incomplete forward-declared types
      if (!RD->isCompleteDefinition()) {
        incompleteTypes.push_back(RD);
      }
    }
    else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumWithType")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Record enums with explicit underlying types
      if (ED->getIntegerTypeSourceInfo()) {
        targetEnums.push_back(ED);
      }
    }
    else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumToMutate")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Get the source code text of target node
      auto enumRange = CharSourceRange::getTokenRange(ED->getSourceRange());
      auto enumText = stringutils::rangetoStr(*(Result.SourceManager), enumRange.getAsRange());
      
      //Find underlying type location
      auto typeSourceInfo = ED->getIntegerTypeSourceInfo();
      if (!typeSourceInfo) return;
      
      auto typeRange = typeSourceInfo->getTypeLoc().getSourceRange();
      auto underlyingTypeText = stringutils::rangetoStr(*(Result.SourceManager), typeRange);
      
      //Select incomplete type
      std::string newTypeName;
      if (!incompleteTypes.empty()) {
        size_t idx = getrandom::getRandomIndex(incompleteTypes.size() - 1);
        newTypeName = incompleteTypes[idx]->getNameAsString();
      } else {
        //Create new forward declaration
        newTypeName = "MutatedIncompleteType";
        Rewrite.InsertTextBefore(ED->getBeginLoc(), "struct " + newTypeName + ";\n");
      }
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = enumText.find(underlyingTypeText);
      if (pos != std::string::npos) {
        enumText.replace(pos, underlyingTypeText.length(), newTypeName);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(enumRange, "/*mut180*/" + enumText);
      }
    }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher incompleteMatcher = recordDecl(unless(isDefinition())).bind("incompleteType");
    DeclarationMatcher enumMatcher = enumDecl().bind("enumWithType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(incompleteMatcher, &callback);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.matchAST(Context);
    
    //Second pass to apply mutations after collecting all incomplete types
    if (!callback.targetEnums.empty()) {
      MatchFinder mutateFinder;
      size_t idx = getrandom::getRandomIndex(callback.targetEnums.size() - 1);
      auto targetEnum = callback.targetEnums[idx];
      
      //Create matcher for specific enum to mutate
      auto enumName = targetEnum->getNameAsString();
      DeclarationMatcher mutateMatcher = enumDecl(hasName(enumName)).bind("enumToMutate");
      mutateFinder.addMatcher(mutateMatcher, &callback);
      mutateFinder.matchAST(Context);
    }
}