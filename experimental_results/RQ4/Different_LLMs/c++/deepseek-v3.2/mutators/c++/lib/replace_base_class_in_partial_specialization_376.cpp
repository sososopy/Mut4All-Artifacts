//source file
#include "../include/Mutator_Replace_Base_Class_In_Partial_Specialization_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("PartialSpecialization")) {
      //Filter nodes in header files
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      if (PS->getSpecializationKind() != clang::TSK_ExplicitSpecialization)
        return;
      if (!PS->getNumBases())
        return;
      
      //Get the source code text of target node
      auto primaryTemplate = PS->getSpecializedTemplate();
      if (!primaryTemplate)
        return;
      
      //Collect available types and template parameters
      if (availableTypes.empty()) {
        auto decls = Result.Context->getTranslationUnitDecl()->decls();
        for (auto decl : decls) {
          if (auto *recordDecl = dyn_cast<clang::CXXRecordDecl>(decl)) {
            if (recordDecl->hasNameForLinkage() && !recordDecl->isLambda()) {
              availableTypes.push_back(recordDecl->getNameAsString());
            }
          }
        }
        //Add fundamental types
        availableTypes.push_back("int");
        availableTypes.push_back("float");
        availableTypes.push_back("double");
        availableTypes.push_back("char");
      }
      
      if (availableTemplateParams.empty()) {
        auto templateParams = primaryTemplate->getTemplateParameters();
        for (unsigned i = 0; i < templateParams->size(); ++i) {
          auto param = templateParams->getParam(i);
          if (param->isTemplateParameter())
            availableTemplateParams.push_back(param->getNameAsString());
        }
      }
      
      //Get base class
      auto baseSpec = PS->bases_begin();
      if (baseSpec == PS->bases_end())
        return;
      
      auto baseType = baseSpec->getType();
      std::string baseTypeStr = baseType.getAsString();
      
      //Perform mutation on the source code text by applying string replacement
      std::string newBaseType;
      int choice = getrandom::getRandomIndex(2);
      
      if (choice == 0 && !availableTemplateParams.empty()) {
        //Replace with template parameter from primary template
        size_t paramIndex = getrandom::getRandomIndex(availableTemplateParams.size() - 1);
        newBaseType = availableTemplateParams[paramIndex];
      } else {
        //Replace with concrete type
        size_t typeIndex = getrandom::getRandomIndex(availableTypes.size() - 1);
        newBaseType = availableTypes[typeIndex];
      }
      
      //Replace the original AST node with the mutated one
      SourceRange baseRange = baseSpec->getSourceRange();
      Rewrite.ReplaceText(baseRange, newBaseType);
    }
}
  
void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}