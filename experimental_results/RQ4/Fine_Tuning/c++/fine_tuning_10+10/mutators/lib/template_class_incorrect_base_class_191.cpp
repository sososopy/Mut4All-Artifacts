//source file
#include "../include/Mutator_Template_Class_Incorrect_Base_Class_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      
      //Get the source code text of target node
      auto baseSpecifier = TS->bases_begin();
      if (baseSpecifier != TS->bases_end()) {
        auto baseTypeLoc = baseSpecifier->getTypeSourceInfo()->getTypeLoc();
        auto baseTypeRange = baseTypeLoc.getSourceRange();
        
        //Perform mutation on the source code text by applying string replacement
        std::string newBaseClass = "NonExistentClass";
        Rewrite.ReplaceText(baseTypeRange, newBaseClass);
      }
    }
}
  
void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}