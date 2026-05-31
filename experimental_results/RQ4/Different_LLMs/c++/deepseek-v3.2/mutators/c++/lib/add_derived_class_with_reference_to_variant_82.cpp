//source file
#include "../include/Mutator_Add_Derived_Class_With_Reference_To_Variant_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("variantDecl")) {
      //Filter nodes in header file
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      const clang::Type* varType = VD->getType().getTypePtr();
      const clang::TemplateSpecializationType* templateType = varType->getAs<clang::TemplateSpecializationType>();
      if (!templateType)
        return;
      
      const clang::TemplateDecl* templateDecl = templateType->getTemplateName().getAsTemplateDecl();
      if (!templateDecl || templateDecl->getName() != "variant")
        return;
      
      // Collect available types from variant alternatives
      for (unsigned i = 0; i < templateType->getNumArgs(); ++i) {
        const clang::TemplateArgument& arg = templateType->getArg(i);
        if (arg.getKind() == clang::TemplateArgument::Type) {
          clang::QualType altType = arg.getAsType();
          availableTypes.push_back(altType.getTypePtr());
          if (const clang::RecordType* recordType = altType->getAs<clang::RecordType>()) {
            if (const clang::CXXRecordDecl* recordDecl = clang::dyn_cast<clang::CXXRecordDecl>(recordType->getDecl())) {
              if (recordDecl->isCompleteDefinition()) {
                baseClasses.push_back(recordDecl);
              }
            }
          }
        }
      }
      
      if (baseClasses.empty() || availableTypes.empty())
        return;
      
      // Choose a base class randomly
      size_t baseIdx = getrandom::getRandomIndex(baseClasses.size() - 1);
      const clang::CXXRecordDecl* baseClass = baseClasses[baseIdx];
      
      // Choose a referenced type randomly from available types
      size_t refIdx = getrandom::getRandomIndex(availableTypes.size() - 1);
      const clang::Type* refType = availableTypes[refIdx];
      
      // Generate derived class name
      std::string derivedName = "Derived_" + baseClass->getNameAsString() + "_WithRef";
      
      // Create derived class definition with reference member
      std::string derivedClass = "\nstruct " + derivedName + " : public " + baseClass->getNameAsString() + " {\n";
      derivedClass += "    " + refType->getCanonicalTypeInternal().getAsString() + "& ref_member;\n";
      derivedClass += "    " + derivedName + "(" + refType->getCanonicalTypeInternal().getAsString() + "& r) : ref_member(r) {}\n";
      derivedClass += "};\n";
      
      // Insert derived class before variant declaration
      TheRewriter.InsertTextBefore(VD->getBeginLoc(), "/*mut82*/" + derivedClass);
      
      // Modify variant type to include derived class
      std::string variantText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
      std::string typeStr = VD->getType().getAsString();
      size_t templateStart = typeStr.find('<');
      size_t templateEnd = typeStr.rfind('>');
      
      if (templateStart != std::string::npos && templateEnd != std::string::npos) {
        std::string templateArgs = typeStr.substr(templateStart + 1, templateEnd - templateStart - 1);
        std::string newTemplateArgs = templateArgs + ", " + derivedName;
        std::string newType = "std::variant<" + newTemplateArgs + ">";
        
        // Replace the type in the variant declaration
        size_t typePos = variantText.find(typeStr);
        if (typePos != std::string::npos) {
          variantText.replace(typePos, typeStr.length(), newType);
          TheRewriter.ReplaceText(clang::CharSourceRange::getTokenRange(VD->getSourceRange()), variantText);
        }
      }
    }
    else if (auto *RT = Result.Nodes.getNodeAs<clang::RecordType>("recordType")) {
      // Collect all record types for potential reference members
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getDecl()->getLocation()))
        return;
      
      availableTypes.push_back(RT);
    }
}
  
void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher variantMatcher = varDecl(hasType(cxxRecordDecl(hasName("std::variant")))).bind("variantDecl");
    TypeMatcher recordMatcher = recordType().bind("recordType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(variantMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}