//source file
#include "../include/Mutator_Variadic_Friend_Operator_Template_With_Mismatched_Pack_Expansion_Lengths_In_Requires_Clause_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("VariadicTemplateStruct")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      
      auto *Record = CTD->getTemplatedDecl();
      if (!Record || !Record->isCompleteDefinition())
        return;
      
      // Check if it's a variadic template
      auto *TemplateParams = CTD->getTemplateParameters();
      bool isVariadic = false;
      for (unsigned i = 0; i < TemplateParams->size(); ++i) {
        if (TemplateParams->getParam(i)->isParameterPack()) {
          isVariadic = true;
          break;
        }
      }
      if (!isVariadic)
        return;
      
      // Avoid duplicate mutations
      for (auto *Visited : visitedTemplates) {
        if (Visited == CTD)
          return;
      }
      visitedTemplates.push_back(CTD);
      
      // Get the source code text of target node
      auto StructRange = Record->getSourceRange();
      if (StructRange.isInvalid())
        return;
      
      // Check if concept C exists, if not add it
      bool conceptExists = false;
      auto &Context = *Result.Context;
      auto *TU = Context.getTranslationUnitDecl();
      for (auto *Decl : TU->decls()) {
        if (auto *ConceptDecl = llvm::dyn_cast<clang::ConceptDecl>(Decl)) {
          if (ConceptDecl->getName() == "C") {
            conceptExists = true;
            break;
          }
        }
      }
      
      if (!conceptExists) {
        std::string ConceptDecl = "\ntemplate<typename, typename> concept C = true;\n";
        Rewrite.InsertTextBefore(TU->getBeginLoc(), ConceptDecl);
      }
      
      // Perform mutation: insert friend operator template with requires clause
      std::string FriendOperator = "\n\ttemplate<typename... Us> requires(... and C<Ts, Us>) friend bool operator==(S, S<Us...>) { return true; }\n";
      
      // Find the insertion point before the closing brace
      SourceLocation InsertLoc = Record->getBraceRange().getEnd();
      if (InsertLoc.isInvalid())
        return;
      
      // Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(InsertLoc, FriendOperator);
      
      // Also add an explicit instantiation with different number of arguments
      std::string Instantiation = "\n/*mut125*/template struct S<int>;\n";
      Rewrite.InsertTextAfterToken(InsertLoc, Instantiation);
    }
}
  
void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isStruct()))).bind("VariadicTemplateStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}