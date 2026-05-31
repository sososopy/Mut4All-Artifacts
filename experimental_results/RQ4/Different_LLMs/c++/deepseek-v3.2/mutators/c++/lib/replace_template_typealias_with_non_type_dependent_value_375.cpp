//source file
#include "../include/Mutator_Replace_Template_Typealias_With_Non_Type_Dependent_Value_375.h"

// ========================================================================================================
#define MUT375_OUTPUT 1

void MutatorFrontendAction_375::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplate")) {
        //Filter nodes in header files
        if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TA->getLocation()))
            return;
        
        //Get the source code text of target node
        const clang::TypeAliasDecl* aliasDecl = TA->getTemplatedDecl();
        if (!aliasDecl) return;
        
        clang::QualType underlyingType = aliasDecl->getUnderlyingType();
        const clang::DependentNameType* depNameType = underlyingType->getAs<clang::DependentNameType>();
        if (!depNameType) return;
        
        const clang::NestedNameSpecifier* nns = depNameType->getQualifier();
        if (!nns) return;
        
        const clang::Type* qualifierType = nns->getAsType();
        if (!qualifierType) return;
        
        const clang::TemplateSpecializationType* templateSpecType = qualifierType->getAs<clang::TemplateSpecializationType>();
        if (!templateSpecType) return;
        
        const clang::TemplateDecl* templateDecl = templateSpecType->getTemplateName().getAsTemplateDecl();
        if (!templateDecl) return;
        
        const clang::ClassTemplateDecl* classTemplateDecl = llvm::dyn_cast<clang::ClassTemplateDecl>(templateDecl);
        if (!classTemplateDecl) return;
        
        std::string templateName = classTemplateDecl->getNameAsString();
        std::string memberName = depNameType->getIdentifier()->getName().str();
        
        //Check if the referenced template already has a static non-type member
        const clang::CXXRecordDecl* recordDecl = classTemplateDecl->getTemplatedDecl();
        if (!recordDecl) return;
        
        bool hasNonTypeMember = false;
        std::string nonTypeMemberName;
        
        for (const clang::Decl* d : recordDecl->decls()) {
            if (const clang::VarDecl* varDecl = llvm::dyn_cast<clang::VarDecl>(d)) {
                if (varDecl->isStaticDataMember()) {
                    hasNonTypeMember = true;
                    nonTypeMemberName = varDecl->getNameAsString();
                    break;
                }
            }
        }
        
        //If no non-type member exists, add one
        if (!hasNonTypeMember) {
            //Find the template definition location
            SourceLocation insertLoc = recordDecl->getEndLoc().getLocWithOffset(-1);
            std::string insertion = "\n    static constexpr int v = 0;";
            Rewrite.InsertTextBefore(insertLoc, insertion);
            nonTypeMemberName = "v";
            //Store template info for later reference
            templateDecls.push_back(classTemplateDecl);
            templateNames.push_back(templateName);
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string originalAliasText = stringutils::rangetoStr(*(Result.SourceManager), TA->getSourceRange());
        std::string mutatedAliasText = originalAliasText;
        
        //Replace the type member reference with non-type member reference
        size_t pos = mutatedAliasText.find(memberName);
        if (pos != std::string::npos) {
            mutatedAliasText.replace(pos, memberName.length(), nonTypeMemberName);
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), mutatedAliasText);
    }
}
  
void MutatorFrontendAction_375::MutatorASTConsumer_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TypeAliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}