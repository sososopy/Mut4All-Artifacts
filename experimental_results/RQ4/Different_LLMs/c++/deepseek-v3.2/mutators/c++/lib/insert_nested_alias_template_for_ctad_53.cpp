//source file
#include "../include/MutatorFrontendAction_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition() || !MT->getDescribedClassTemplate())
        return;
      // Check if the template class has at least one constructor
      bool hasConstructor = false;
      for (auto *D : MT->decls()) {
        if (auto *Ctor = dyn_cast<CXXConstructorDecl>(D)) {
          if (!Ctor->isImplicit()) {
            hasConstructor = true;
            break;
          }
        }
      }
      if (!hasConstructor)
        return;
      //Record the node information to be used in the mutation process
      TargetTemplateClass = MT;
      // Determine template parameter type (assuming single type parameter for simplicity)
      if (auto *TemplateParms = MT->getDescribedClassTemplate()->getTemplateParameters()) {
        if (TemplateParms->size() > 0) {
          if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(*TemplateParms->begin())) {
            TemplateParamType = TTP->getNameAsString();
            if (TemplateParamType.empty())
              TemplateParamType = "T";
          }
        }
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ValueDecl")) {
      if (!TargetTemplateClass)
        return;
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      // Check if the variable type matches the template parameter type
      QualType VarType = VD->getType();
      std::string VarTypeStr = VarType.getAsString();
      if (VarTypeStr.find(TemplateParamType) != std::string::npos ||
          (TemplateParamType == "T" && VarTypeStr.find("int") != std::string::npos)) {
        //Get the source code text of target node
        ValueToUse = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      if (!TargetTemplateClass || ValueToUse.empty())
        return;
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string ClassName = TargetTemplateClass->getNameAsString();
      std::string Insertion = "\ntemplate<typename U> using Proxy_53 = U;\n";
      Insertion += "template<typename U> using Alias_53 = Proxy_53< " + ClassName + "<U> >;\n";
      // Insert alias templates before the function
      Rewrite.InsertText(FD->getBeginLoc(), Insertion);
      // Insert CTAD variable declaration inside the function body
      std::string CTADDecl = "\n    Alias_53 var_53{ " + ValueToUse + " };\n";
      if (auto *Body = FD->getBody()) {
        Rewrite.InsertText(Body->getBeginLoc().getLocWithOffset(1), CTADDecl);
      }
      // Reset for next potential mutation
      TargetTemplateClass = nullptr;
      ValueToUse.clear();
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = cxxRecordDecl(isDefinition(), hasDescendant(cxxConstructorDecl().bind("Ctor"))).bind("TemplateClass");
    DeclarationMatcher matcher2 = varDecl().bind("ValueDecl");
    DeclarationMatcher matcher3 = functionDecl(hasBody(compoundStmt())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
}